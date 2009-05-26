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
		ft_keyboard.c \
		ft_matrix.c \
		ft_lcdcolor.c \
		ft_main.c
 
LOCAL_MODULE := factory_test

include $(BUILD_EXECUTABLE)

