#pragma once
#include <unordered_map>

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
        TritSet(std::size_t size);
        TritSet(const TritSet& set);
        ~TritSet();
        std::size_t underlaying_capacity() const; // size of underlaying array, incapsulation leak actually
        std::size_t capacity() const;
        std::size_t length() const; //ind of first !U + 1
        
        reference operator [](std::size_t index);
        Trit operator [](std::size_t index) const;
        void shrink();

        TritSet& operator &=(const TritSet& A);
        TritSet& operator |=(const TritSet& A);
        TritSet& operator ~();

        int cardinality(Trit value);
        std::unordered_map< Trit, int > cardinality();
        void trim(size_t lastIndex);
};

TritSet operator & (const TritSet& A, const TritSet& B);
TritSet operator | (const TritSet& A, const TritSet& B);