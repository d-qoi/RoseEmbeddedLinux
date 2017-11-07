#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xbba89470, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x9978f7df, __VMLINUX_SYMBOL_STR(param_ops_short) },
	{ 0xfe990052, __VMLINUX_SYMBOL_STR(gpio_free) },
	{ 0xf20dabd8, __VMLINUX_SYMBOL_STR(free_irq) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xd6b8e852, __VMLINUX_SYMBOL_STR(request_threaded_irq) },
	{ 0x65cfafe4, __VMLINUX_SYMBOL_STR(gpiod_to_irq) },
	{ 0x1e438886, __VMLINUX_SYMBOL_STR(gpiod_export) },
	{ 0xaa507406, __VMLINUX_SYMBOL_STR(gpiod_set_debounce) },
	{ 0x49c59a59, __VMLINUX_SYMBOL_STR(gpiod_direction_input) },
	{ 0x47229b5c, __VMLINUX_SYMBOL_STR(gpio_request) },
	{ 0x1a0947df, __VMLINUX_SYMBOL_STR(spi_add_device) },
	{ 0x43ffb4a2, __VMLINUX_SYMBOL_STR(device_unregister) },
	{ 0xddb4a2a0, __VMLINUX_SYMBOL_STR(bus_find_device_by_name) },
	{ 0xb81960ca, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0x8b2412fc, __VMLINUX_SYMBOL_STR(put_device) },
	{ 0xbf0ce3fe, __VMLINUX_SYMBOL_STR(spi_alloc_device) },
	{ 0x71c12d92, __VMLINUX_SYMBOL_STR(spi_busnum_to_master) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xce01f638, __VMLINUX_SYMBOL_STR(gpiod_get_raw_value) },
	{ 0x9af2fd11, __VMLINUX_SYMBOL_STR(gpio_to_desc) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "192FB9A310BA35A4D0368E3");
