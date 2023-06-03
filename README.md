# Executor

A simple shell command executor written in C++, the alternative of subprocess or os.system in Python.


## Example

- Simple (congested):

```cpp
#include <iostream>

#include "executor.hpp"

int main() {
    const char* cmd = "echo \"Hello World!\"";
    std::cout << ubn::executor::executor(cmd)->get()->str();
}
```

- Async:

```cpp
#include <iostream>

#include "executor.hpp"

int main() {
    const char* cmd = "ping 8.8.8.8 -c 4";
    auto exec_ftr = ubn::executor::executor(cmd);
    
    // do other things, command executes in the backgroud
    
    std::cout << exec_ftr->get()->str();
}
```

- Advanced:

```cpp
#include <iostream>

#include "executor.hpp"

int main() {
    const char* cmd = "ping 8.8.8.8 -c 4";
    auto exec_ftr = ubn::executor::executor<128ul>(ubn::exector::policy::async_wait, cmd);   
    std::cout << exec_ftr->get()->str();
}
```


## Template Parameters

- `ubn::exector::policy`: `async_wait`
- `ubn::exector::errors`: `no_implementation`, `popen_failed`
- `ubn::exector::buffer_size`: `64ul`


## TODO

1. Support `ubn::exector::policy::async_wait_for`
2. Support `ubn::exector::policy::async_wait_until`
3. Support `ubn::exector::policy::async_realtime`
4. Support `ubn::exector::policy::sync_wait`
5. Support `ubn::exector::policy::sync_wait_for`
6. Support `ubn::exector::policy::sync_wait_until`
7. Support `ubn::exector::policy::sync_realtime`


## License

MIT License Copyright (c) 2023 Unbinilium.
