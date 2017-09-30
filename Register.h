#include <cstdint>

void foo();

class Register {
private:
    template<class=void>
    uint32_t valueBuilder(uint32_t value) {
        return value;
    }
    template<class T, class... Tail>
    uint32_t valueBuilder(uint32_t value, uint_fast8_t bit, Tail... tail) {
        value |= (1 << bit);
        return valueBuilder<Tail...>(value, tail...);
    }
public:
    Register(uintptr_t address) : mAddress((uint32_t*)address) {
    }
    Register(const Register&) = delete;
    Register& operator=(const Register&) = delete;

    void write(uint32_t value) {
        *mAddress = value;
    }
    void write(uint32_t value, uint_fast8_t shift) {
        *mAddress |= (value << shift);
    }
    void writeOnly(uint32_t value, uint_fast8_t shift) {
        *mAddress = (value << shift);
    }
    uint32_t read() const {
        return *mAddress;
    }
    uint32_t read(uint_fast8_t shift, uint_fast8_t numBits) const {
        return (*mAddress >> shift) & ((1 << numBits)-1);
    }
    bool get(uint_fast8_t bit) {
        return *mAddress & (1 << bit);
    }
    template<class... Ts>
    void set(uint_fast8_t bit, Ts... bits) {
        *mAddress |= valueBuilder<Ts...>(0, bit, bits...);
    }
    void set() {
        *mAddress = 0xffffffff;
    }
    template<class... Ts>
    void setOnly(uint_fast8_t bit, Ts... bits) {
        *mAddress = valueBuilder<Ts...>(0, bit, bits...);
    }
    template<class...Ts>
    void clear(uint_fast8_t bit, Ts... bits) {
        *mAddress &= ~valueBuilder<Ts...>(0, bit, bits...);
    }
    void clear() {
        *mAddress = 0;
    }
    operator uint32_t() const {
        return read();
    }
private:
    volatile uint32_t* mAddress;
};
