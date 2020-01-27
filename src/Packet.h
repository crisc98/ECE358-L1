class Packet {
    private:
    long arrivalTick;
    bool servicing = false;
    long departureTick = 0;

    public:
    Packet(long tick) {
        arrivalTick = tick;
    }

    long getArrivalTick() {
        return arrivalTick;
    }

    long getDepartureTick() {
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