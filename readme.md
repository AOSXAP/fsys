
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
- file.lua code:

```lua
local fsys = require("fsys");

e = fsys.scan("./test_dir"); 

function scan(e, padding)
    for k, v in pairs(e) do 
        for i = 0 , padding - 1 do 
            io.write("\t")
        end
        io.write(v, '\n')

        if(fsys.is_dir(v) == true) then
            q = fsys.scan(v); 
            scan(q , padding + 1)    
        end

    end
end

scan(e, 0)
```
