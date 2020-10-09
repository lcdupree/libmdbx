#include "test.h"

void testcase_copy::copy_db(const bool with_compaction) {
  int err = mdbx_env_delete(copy_pathname.c_str(), MDBX_ENV_JUST_DELETE);
  if (err != MDBX_SUCCESS && err != MDBX_RESULT_TRUE)
    failure_perror("mdbx_removefile()", err);

  err = mdbx_env_copy(db_guard.get(), copy_pathname.c_str(),
                      with_compaction ? MDBX_CP_COMPACT : MDBX_CP_DEFAULTS);
  if (unlikely(err != MDBX_SUCCESS))
    failure_perror(with_compaction ? "mdbx_env_copy(MDBX_CP_COMPACT)"
                                   : "mdbx_env_copy(MDBX_CP_ASIS)",
                   err);
}

bool testcase_copy::run() {
  jitter_delay();
  db_open();
  assert(!txn_guard);
  const bool order = flipcoin();
  jitter_delay();
  copy_db(order);
  jitter_delay();
  copy_db(!order);
  return true;
}
