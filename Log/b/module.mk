app_dirs	:= \
	app/src \
	app/Logging/src

#app_SRCS    := $(call fn-find-srcs, app/src, $(blacklist))
#app_ut_SRCS := $(call fn-find-srcs, app/test-src, app/test-src/ct_%)

ifeq ($(arch), elc64)
	app_dirs += \
		app/Logging/platform/elc
else ifeq ($(filter $(arch), seos-ssc seos-vssc), $(arch))
	app_dirs += \
		app/Logging/platform/seos
else
$(error Unsupported architecture $(arch))
endif

ifeq ($(AutoMock), 1)
blacklist       := app/Logging/src/app_lttng_debuglog_provider.c
else
blacklist	:= $(if $(and $(findstring seos, $(arch)), $(filter $(CONFIG_LTTNG_SSR), y)),,app/Logging/src/app_lttng_debuglog_provider.c)
endif

app_SRCS        := $(call fn-find-srcs, $(app_dirs))
app_SRCS        := $(filter-out $(blacklist),$(app_SRCS))
app_ut_SRCS     := $(call fn-find-srcs, app/test-src, app/test-src/ct_%)

