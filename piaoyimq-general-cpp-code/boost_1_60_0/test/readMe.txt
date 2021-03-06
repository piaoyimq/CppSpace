Custom command:
./a.out -x -l all
./a.out -x -t length_test -l all
./a.out -x -t length_test -l all -p
./a.out --list_content   #list the test cases.
./a.out --report_level=detailed
./a.out  -x -l all -t test_suite_common/test_getNameBy*
./a.out -x -l message   --random

boost_runtime_list_content.run-fail.cpp
decorator_07.run-fail.cpp
decorator_12.run.cpp
example18.run-fail.cpp
example24.run.cpp
example28.run-fail.cpp
example29.run-fail.cpp
example31.run-fail.cpp
example50.run-fail.cpp
example52.run-fail.cpp
example82_contexts.run-fail.cpp
exception_api.run-fail.cpp
exception_check_predicate.run-fail.cpp





Usage: a.out [Boost.Test argument]... -- [custom test module argument]...

Boost.Test arguments correspond to parameters listed below. All parameters are optional. You can use specify parameter value either as a command line argument or as a value of corresponding environment variable. In case if argument for the same parameter is specified in both places, command line is taking precedence. Command line argument format supports parameter name guessing, so you can use any unambiguous prefix to identify a parameter. All the arguments after the -- are ignored by the Boost.Test.

Boost.Test supports following parameters:
Parameter: auto_start_dbg
 Automatically attaches debugger in case of system level failure (signal).
 Command line formats:
   --auto_start_dbg[=<boolean value>]
   -d[ <boolean value>]
 Environment variable: BOOST_TEST_AUTO_START_DBG
Parameter: break_exec_path
 For the exception safety testing allows to break at specific execution path.
 Command line formats:
   --break_exec_path=<value>
 Environment variable: BOOST_TEST_BREAK_EXEC_PATH
Parameter: build_info
 Displays library build information.
 Command line formats:
   --build_info[=<boolean value>]
   -i[ <boolean value>]
 Environment variable: BOOST_TEST_BUILD_INFO
Parameter: catch_system_errors
 Allows to switch between catching and ignoring system errors (signals).
 Command line formats:
   --[no_]catch_system_errors[=<boolean value>]
   -s[ <boolean value>]
 Environment variable: BOOST_TEST_CATCH_SYSTEM_ERRORS
Parameter: color_output
 Enables color output of the framework log and report messages.
 Command line formats:
   --[no_]color_output[=<boolean value>]
   -x[ <boolean value>]
 Environment variable: BOOST_TEST_COLOR_OUTPUT
Parameter: detect_fp_exceptions
 Enables/disables floating point exceptions traps.
 Command line formats:
   --[no_]detect_fp_exceptions[=<boolean value>]
 Environment variable: BOOST_TEST_DETECT_FP_EXCEPTIONS
Parameter: detect_memory_leaks
 Turns on/off memory leaks detection (optionally breaking on specified alloc order number).
 Command line formats:
   --detect_memory_leaks[=<alloc order number>]
 Environment variable: BOOST_TEST_DETECT_MEMORY_LEAK
Parameter: help
 Help for framework parameters.
 Command line formats:
   --help[=<parameter name>]
Parameter: list_content
 Lists the content of test tree - names of all test suites and test cases.
 Command line formats:
   --list_content[=<HRF|DOT>]
 Environment variable: BOOST_TEST_LIST_CONTENT
Parameter: list_labels
 Lists all available labels.
 Command line formats:
   --list_labels[=<boolean value>]
 Environment variable: BOOST_TEST_LIST_LABELS
Parameter: log_format
 Specifies log format.
 Command line formats:
   --log_format=<HRF|CLF|XML>
   -f <HRF|CLF|XML>
 Environment variable: BOOST_TEST_LOG_FORMAT
Parameter: log_level
 Specifies log level.
 Command line formats:
   --log_level=<all|success|test_suite|unit_scope|message|warning|error|cpp_exception|system_error|fatal_error|nothing>
   -l <all|success|test_suite|unit_scope|message|warning|error|cpp_exception|system_error|fatal_error|nothing>
 Environment variable: BOOST_TEST_LOG_LEVEL
Parameter: log_sink
 Specifies log sink: stdout(default), stderr or file name.
 Command line formats:
   --log_sink=<stderr|stdout|file name>
   -k <stderr|stdout|file name>
 Environment variable: BOOST_TEST_LOG_SINK
Parameter: output_format
 Specifies output format (both log and report).  Command line formats: --output_format=<HRF|CLF|XML>
   -o <HRF|CLF|XML>
 Environment variable: BOOST_TEST_OUTPUT_FORMAT
Parameter: random
 Allows to switch between sequential and random order of test units execution. Optionally allows to specify concrete seed for random number generator.
 Command line formats:
   --random[=<seed>]
 Environment variable: BOOST_TEST_RANDOM Parameter: report_format
 Specifies report format.
 Command line formats:
   --report_format=<HRF|CLF|XML>
   -m <HRF|CLF|XML>
 Environment variable: BOOST_TEST_REPORT_FORMAT
Parameter: report_level
 Specifies report level.
 Command line formats:
   --report_level=<confirm|short|detailed|no>
   -r <confirm|short|detailed|no>
 Environment variable: BOOST_TEST_REPORT_LEVEL
Parameter: report_memory_leaks_to
 File where to report memory leaks to.
 Command line formats:
   --report_memory_leaks_to=<file name>
 Environment variable: BOOST_TEST_REPORT_MEMORY_LEAKS_TO
Parameter: report_sink
 Specifies report sink: stderr(default), stdout or file name.
 Command line formats:
   --report_sink=<stderr|stdout|file name>
   -e <stderr|stdout|file name>
 Environment variable: BOOST_TEST_REPORT_SINK
Parameter: result_code
 Disables test modules's result code generation.
 Command line formats:
   --[no_]result_code[=<boolean value>]
   -c[ <boolean value>]
 Environment variable: BOOST_TEST_RESULT_CODE
Parameter: run_test
 Filters, which test units to include or exclude from test module execution.
 Command line formats:
   --run_test=<test unit filter>
   -t <test unit filter>
 Environment variable: BOOST_TEST_RUN_FILTERS
Parameter: save_pattern
 Allows to switch between saving or matching test pattern file.
 Command line formats:
   --save_pattern[=<boolean value>]
 Environment variable: BOOST_TEST_SAVE_PATTERN
Parameter: show_progress
 Turns on progress display.
 Command line formats:
   --show_progress[=<boolean value>]
   -p[ <boolean value>]
 Environment variable: BOOST_TEST_SHOW_PROGRESS
Parameter: usage
 Short message explaining usage of Boost.Test parameters.
 Command line formats:
   -?[ <boolean value>]
Parameter: use_alt_stack
 Turns on/off usage of an alternative stack for signal handling.
 Command line formats:
   --[no_]use_alt_stack[=<boolean value>]
 Environment variable: BOOST_TEST_USE_ALT_STACK
Parameter: wait_for_debugger
 Forces test module to wait for button to be pressed before starting test run.
 Command line formats:
   --wait_for_debugger[=<boolean value>]
   -w[ <boolean value>]
 Environment variable: BOOST_TEST_WAIT_FOR_DEBUGGER

Use --help=<parameter name> to display detailed help for specific parameter.
