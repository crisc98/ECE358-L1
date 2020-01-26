class Packet {
    private:
    long arrivalTick;
    bool servicing = false;
    long departureTick = 0;

    public:
    Packet(long arrivalTick) {
        arrivalTick = arrivalTick;
    }

    long arrivalTick() {
        return arrivalTick;
    }

    long departureTick() {
        return departureTick;
    }

    bool isServicing() {
        return servicing;
    }

    void service(long tick) {
        servicing = true;
        departureTick = tick;
    }
};