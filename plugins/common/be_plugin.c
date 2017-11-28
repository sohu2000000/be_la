#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "be_plugin.h"
#include "be_la_log.h"
#include "be_acc_card.h"

acc_plugin_t g_acc_plugins[VENDOR_MAX][MODEL_MAX];

int be_plugin_register(acc_plugin_t* plugin) {
	assert(plugin);
	assert(plugin->pfunc);
	assert(plugin->model < MODEL_MAX && plugin->model > 0);
	assert(plugin->vendor < VENDOR_MAX && plugin->vendor > 0);

	if (be_plugin_is_registed(plugin->vendor, plugin->model)) {
		BE_LA_ERROR("plugin %s has already registed!\n", plugin->name);
		return -1;
	}

	memcpy(&g_acc_plugins[plugin->vendor][plugin->model], plugin,
			sizeof(*plugin));
	BE_LA_LOG("plugin %s install success!\n",
			g_acc_plugins[plugin->vendor][plugin->model].name);
	return 0;
}

int be_plugin_unregister(acc_plugin_t* plugin) {
	assert(plugin);
	assert(plugin->pfunc);

	if (be_plugin_is_registed(plugin->vendor, plugin->model)) {
		memset(&g_acc_plugins[plugin->vendor][plugin->model], 0,
				sizeof(*plugin));
	}

	BE_LA_LOG("plugin %s uninstall success!\n",
			g_acc_plugins[plugin->vendor][plugin->model].name);
	return 0;
}

int be_plugin_is_registed(enum tag_vendor vendor, enum tag_model model) {
	if (g_acc_plugins[vendor][model].pfunc) {
		return 1;
	}
	return 0;
}

#if 0
int be_plugin_callback(be_card_inner_t*card, void*inbuf, uint32_t in_len,
		void*outbuf, int32_t*out_len) {
	int ret;

	assert(card);
	acc_plugin_t* plugin = &g_acc_plugins[card->card.vendor][card->card.model];

	BE_LA_DEBUG("plugin %s process message", plugin->name);
	ret = plugin->pfunc(card, inbuf, in_len, outbuf, out_len);
	BE_LA_DEBUG("plugin %s process message end", plugin->name);
	return ret;
}
#endif

acc_plugin_t* be_plugin_match(acc_card_t*card) {
	assert(card);
	acc_plugin_t* plugin = &g_acc_plugins[card->vendor][card->model];
	if (plugin && ((plugin->acc_type & card->acc_type) == card->acc_type)) {
		return plugin;
	}

	return NULL;
}

void* be_plugin_context_init(be_card_inner_t*card, acc_plugin_t*plugin) {
	assert(plugin);
	assert(card);

	return plugin->context_init(card);
}

int be_plugin_context_destory(be_card_inner_t*card, acc_plugin_t*plugin) {
	assert(plugin);
	assert(card);
	return plugin->context_destory(card);
}

int be_plugin_init(acc_plugin_t* plugin) {
	assert(plugin);
	return plugin->init();
}

int be_plugin_destory(acc_plugin_t* plugin) {
	return -1;
}

int be_plugin_process(be_card_inner_t*card, acc_plugin_t* plugin, void*inbuf,
		uint32_t in_len, void*outbuf, int32_t*out_len) {

	assert(card);
	assert(plugin);
	assert(inbuf);
	assert(outbuf);
	assert(out_len);

	//acc_plugin_t* plugin = &g_acc_plugins[card->card.vendor][card->card.model];

	int ret;
	BE_LA_DEBUG("plugin %s process message", plugin->name);
	ret = plugin->pfunc(card, inbuf, in_len, outbuf, out_len);
	BE_LA_DEBUG("plugin %s process message end", plugin->name);
	return ret;
}
