struct Slot {
    uint32_t tag;
    bool valid;
    uint32_t load_ts, access_ts;
};

struct Set {
    vector<Slot> slots; //Not dynamic (No add, rem), created up front
};

struct Cache {
    vector<Set> sets;
};