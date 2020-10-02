#pragma once

enum class Trit : int{Unknown = 0, False = 1, True = 2};


class TritSet{
    private:
        std::size_t min_size, actual_size;
        std::size_t * array;
        //std::size_t ind_to_chunk(std::size_t index);
        void resize(std::size_t index);
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
        std::size_t capacity(); //size_t's in array
        reference operator [](std::size_t index);
        void shrink();
};