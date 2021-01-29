int lockRegion(int fd,int type,int whence,int start,off_t len);
int lockRegionWait(int fd,int type,int whence,int start,off_t len);
pid_t regionIsLocked(int fd,int type,int whence,int start,off_t len);
