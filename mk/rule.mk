#SUB_MODULE=
#MODULE_BEFORE=
#MODULE_AFTER=
#CLEAN_AFTER=

-include $(BE_ROOT)/mk/config.mk

objects :=$(patsubst %.c,%.o,$(wildcard *.c))
objects-depends :=$(patsubst %.o,%.d,$(objdects))

all:__module_before__ __mk_submod__ __all__ __module_after__
clean:__cl_submod__ __clean__ __module_clean_after__

__all__: $(objects)
__clean__:
	rm -rf $(objects)
	
ifeq ($(strip $(MODULE_BEFORE)),)
__module_before__:
else
__module_before__: $(MODULE_BEFORE)
endif

ifeq ($(strip $(MODULE_AFTER)),)
__module_after__:
else
__module_after__: $(MODULE_AFTER)
endif

ifeq ($(strip $(CLEAN_AFTER)),)
__module_clean_after__:
else
__module_clean_after__: $(CLEAN_AFTER)
endif

__mk_submod__:
	@for i in $(SUB_MODULE);do $(MAKE) -C $$i all || exit 1 ; done;

__cl_submod__:
	@for i in $(SUB_MODULE);do $(MAKE) -C $$i clean || exit 1 ; done;


-include $(objects-depends)

%.o:%.c
	gcc $(cflags) -MMD -MP -c -o $@ $< $(include_path)
%.o:%.c %.h
	gcc $(cflags) -MMD -MP -c -o $@ $< $(include_path)
