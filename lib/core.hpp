constexpr void Core::link(CoreArray *cores, size_t id, Memory_t *mem)
{
    this->cores = cores;
    this->registers.id = id;
    this->mem = mem;
}

