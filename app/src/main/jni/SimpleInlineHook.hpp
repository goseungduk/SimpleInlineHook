


void rwx_protect(uint64_t *addr);
void AllocateBackupSpace(int64_t (**backup)(const char*, const char*));
void BackupOriginCode(uint64_t* origin, uint64_t* backup, int size);
void InlineHook(uint64_t *origin, uint64_t *replace, uint64_t *pInt);