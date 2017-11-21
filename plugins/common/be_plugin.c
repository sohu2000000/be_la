#include <assert.h>
#include <stdlib.h>

#include "be_plugin.h"

#include "../../common/be_la_log.h"

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

int be_plugin_callback(enum tag_vendor vendor, enum tag_model model, void*inbuf,
		uint32_t in_len, void*outbuf, uint32_t*out_len) {
	int ret;
	acc_plugin_t* plugin = &g_acc_plugins[vendor][model];

	BE_LA_DEBUG("plugin %s process message", plugin->name);
	ret = plugin->pfunc(inbuf, in_len, outbuf, out_len);
	BE_LA_DEBUG("plugin %s process message end", plugin->name);
	return ret;
}
