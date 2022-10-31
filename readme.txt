
# Fsys

Filesystem library implementation for lua

## Still in early stages

**Ref:** https://en.cppreference.com/w/cpp/filesystem




## Contributing

Contributions are always welcome!

See `list.txt` to get started.

Implement any feature (or fix existing ones) , modify `core.cpp` and send a pull request.


## Use

- Compile via command:  g++ ./core.cpp -I/usr/include/lua5.4 -llua -fPIC -shared -o fsys.so (or whatever your lua version is)

`local fsys = require("fsys") \n 
local e = fsys.current_path("./core.cpp"); \n
print(e)`
