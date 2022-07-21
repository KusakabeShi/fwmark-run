fwmark-run
=============

**Usage:**

1. Compile:
```
make
```

2. preload the library and run the command you want to hook:
```
LD_PRELOAD="./ld_fwmark.so" DEFAULT_SO_MARK=8 curl -4 ifconfig.me
```