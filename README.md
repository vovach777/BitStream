# BitStream
Bit writer to buffer and read from buffer for codecs. based on ffmpeg code

```c++
#include "bitstream.hpp"

int main() {
    BitWriter writer;

    writer.writeBit(1);
    writer.writeBit(0);


    writer.flush();
    writer.printBits();

    BitReader reader(writer.data(), writer.size());

    reader.printBits();
    return 0;
}
```
