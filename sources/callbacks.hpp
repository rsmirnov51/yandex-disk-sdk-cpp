template <class Stream>
auto read(char * ptr, size_t size, size_t count, void * userdata) -> size_t {

    auto stream = reinterpret_cast<Stream *>(userdata);
    auto byte_count = count * size;
    stream->read(ptr, byte_count);
    return stream->fail() ? 0 : byte_count;
}

template <class Stream>
auto write(char * ptr, size_t size, size_t count, void * userdata) -> size_t {

    auto stream = reinterpret_cast<Stream *>(userdata);
    auto byte_count = count * size;
    stream->write(ptr, byte_count);
    return stream->fail() ? 0 : byte_count;
}
