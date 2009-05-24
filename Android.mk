LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
 
LOCAL_SRC_FILES:= \
		gui/ft_button.c \
		gui/ft_button.h \
		gui/ft_draw.c \
		gui/ft_draw.h \
		gui/ft_event.c \
		gui/ft_event.h \
		gui/ft_input.c \
		gui/ft_input.h \
		gui/ft_framebuffer.c \
		gui/ft_framebuffer.h \
		gui/ft_list.c \
		gui/ft_list.h \
		gui/ft_widget.c \
		gui/ft_widget.h \
		gui/ft_window.c \
		gui/ft_window.h \
		ft_type.h \
		ft_main.c
 
LOCAL_MODULE := factory_test

include $(BUILD_EXECUTABLE)

