LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= camera.cpp\
				  display.cpp\
				  V4l2.cpp

ifneq ($(TARGET_SIMULATOR),true)
LOCAL_MODULE := camera_test
LOCAL_STATIC_LIBRARIES := libcutils 
include $(BUILD_EXECUTABLE)
endif
