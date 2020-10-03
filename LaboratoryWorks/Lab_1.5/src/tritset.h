#pragma once

enum class Trit : int{Unknown = 0, False = 1, True = 2};


class TritSet{
    private:
        std::size_t min_size, actual_size;
        std::size_t * array; //chunk == size_t containing a number of trits
        void resize(std::size_t index);
    public:
        class reference{
            private:
                int index;
                TritSet& parent;
            public:
                reference(int in_ind, TritSet& in_parent);
                reference& operator = (Trit t);
                operator int(); //for cout
                operator Trit();
        };
        TritSet(std::size_t size); // want some realloc and copy at some moment
        ~TritSet();
        std::size_t underlaying_capacity(); // size of underlaying array, incapsulation leak actually
        std::size_t capacity();
        std::size_t last_significant_index() const;
        reference operator [](std::size_t index);
        Trit operator [](std::size_t index) const;
        TritSet& operator &=(const TritSet& A);
        TritSet& operator |=(const TritSet& A);
        TritSet& operator ~();
        void shrink();
};