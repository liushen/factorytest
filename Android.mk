LOCAL_PATH:= $(call my-dir)

ifneq ($(TARGET_SIMULATOR),true)

include $(CLEAR_VARS)
 
LOCAL_SRC_FILES:= \
		gui/ft_button.c \
		gui/ft_status_button.c \
		gui/ft_draw.c \
		gui/ft_event.c \
		gui/ft_input.c \
		gui/ft_framebuffer.c \
		gui/ft_list.c \
		gui/ft_widget.c \
		gui/ft_window.c \
		hw/hw_comm.c \
		hw/hw_audio.c \
		hw/hw_camera.c \
		hw/hw_bluetooth.c \
		hw/hw_wifi.c \
		hw/hw_led.c \
		hw/hw_fm.c \
		hw/hw_gsm.c \
		ft_textpad.c \
		ft_keyboard.c \
		ft_matrix.c \
		ft_adc.c \
		ft_led.c \
		ft_lcdcolor.c \
		ft_gsm.c \
		ft_config.c \
		ft_main.c
 
LOCAL_MODULE := factory_test
LOCAL_CFLAGS := -DSERIAL_NO="\"0.100\""

LOCAL_C_INCLUDES += $(call include-path-for, bluedroid)
LOCAL_SHARED_LIBRARIES += libbluedroid

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= hw/hw_gsm.c
LOCAL_MODULE := at_test
LOCAL_CFLAGS := -DHW_GSM_TEST

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
file := $(TARGET_OUT)/usr/share/factorytest/sound.wav
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/data/sound.wav | $(ACP)
	$(transform-prebuilt-to-target)
endif

include $(call all-makefiles-under,$(LOCAL_PATH))
