#pragma once

enum class Trit{Unknown = 0, False = 1, True = 2};


class TritSet{
    private:
        std::size_t min_size, actual_size;
        std::size_t * array;
        std::size_t & get_chunk(std::size_t index);
        bool widen_to_fit(std::size_t index);
    public:
        class reference{
            private:
                int index;
                TritSet& parent;
            public:
                reference(int in_ind, TritSet & in_parent);
                reference& operator = (Trit t);
                operator int(); //for cout
                operator Trit();
        };
        TritSet(std::size_t size); // want some realloc and copy at some moment
        ~TritSet();
        std::size_t capacity(); //size_t array size
        //constexpr Trit operator [](std::size_t index) const;
        reference operator [](std::size_t index);
        bool shrink();
};