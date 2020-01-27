class Packet {
    private:
    long arrivalTick;
    bool servicing = false;
    long length = 0;
    long departureTick = 0;

    public:
    Packet(long tick, long packetLength) {
        arrivalTick = tick;
        length = packetLength;
        servicing = false;
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

    long getLength() {
        return length;
    }

    void service(long tick) {
        servicing = true;
        departureTick = tick;
    }
};