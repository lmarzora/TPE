void setUpPageFrameAllocator(uint64_t);
void init_pMemoryAllocator(uint64_t);
void setUpScheduler();
void setUpPaging();
void* setUserModule(void* pAddress);
void mapUserModule(void* vAddress, void* pAddress);
