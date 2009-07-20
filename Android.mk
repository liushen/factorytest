LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
 
LOCAL_SRC_FILES:= \
		gui/ft_button.c \
		gui/ft_draw.c \
		gui/ft_event.c \
		gui/ft_input.c \
		gui/ft_framebuffer.c \
		gui/ft_list.c \
		gui/ft_widget.c \
		gui/ft_window.c \
		hw/hw_comm.c \
		hw/hw_audio.c \
		hw/hw_led.c \
		ft_textpad.c \
		ft_keyboard.c \
		ft_matrix.c \
		ft_adc.c \
		ft_led.c \
		ft_lcdcolor.c \
		ft_main.c
 
LOCAL_MODULE := factory_test
LOCAL_CFLAGS := -DSERIAL_NO="\"0.100\""

include $(BUILD_EXECUTABLE)

file := $(TARGET_OUT)/usr/share/factorytest/sound.wav
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/data/sound.wav | $(ACP)
	$(transform-prebuilt-to-target)
