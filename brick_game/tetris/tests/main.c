#include "suites.h"

#define FILENAME_SIZE 128

int main(void) {
  int number_failed;
  SRunner *sr = srunner_create(test_example_name());

  srunner_add_suite(sr, test_another_example_name());

  srunner_set_log(sr, "./test.log");

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? 1 : 0;
}