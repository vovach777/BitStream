# BitStream
Bit writer to buffer and read from buffer for codecs. based on ffmpeg code

```c++
#inclide <iostream>
#include "bitstream.hpp"

int main() {
    BitWriter writer;

    writer.writeBit(1);
    writer.writeBit(0);
    writer.writeBits(4,0xf);

    writer.flush();
    std::cout << writer;

    BitReader reader(writer.data(), writer.size());

    std::cout << reader;
    return 0;
}
```
