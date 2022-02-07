################################################################################
#
# hid-gadget-keyboard
#
################################################################################

HID_GADGET_KEYBOARD_VERSION = 1.0
HID_GADGET_KEYBOARD_SITE = $(BR2_EXTERNAL_pOOBs4_PATH)/package/hid-gadget-keyboard
HID_GADGET_KEYBOARD_SITE_METHOD = local

define HID_GADGET_KEYBOARD_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D) all
endef

define HID_GADGET_KEYBOARD_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/hid-gadget-keyboard $(TARGET_DIR)/usr/bin/hid-gadget-keyboard
endef

$(eval $(generic-package))
