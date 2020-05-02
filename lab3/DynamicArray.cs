using System;
using System.Text;

namespace lab3
{
    public class DynamicArray<T>
    {
        private const uint BaseCapacity = 16;

        private T[] _items;
        private uint _capacity;
        private uint _size;

        public uint Capacity
        {
            get => _capacity;
            set
            {
                if (value < Size)
                {
                    throw new ArgumentOutOfRangeException();
                }

                var newItems = new T[value];
                if (Size > 0)
                {
                    Array.Copy(_items, newItems, Size);
                }

                _items = newItems;
                _capacity = value;
            }
        }

        public uint Size
        {
            get => _size;
            set
            {
                if (value > Capacity)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _size = value;
            }
        }


        public DynamicArray(uint capacity = BaseCapacity)
        {
            Capacity = capacity;
        }


        public T this[uint index]
        {
            get
            {
                if (index >= Size)
                {
                    throw new IndexOutOfRangeException();
                }

                return _items[index];
            }
            set
            {
                if (index >= Size)
                {
                    throw new IndexOutOfRangeException();
                }

                _items[index] = value;
            }
        }

        public void Add(T item)
        {
            if (Capacity == 0)
            {
                Capacity = BaseCapacity;
            }

            if (Size == Capacity)
            {
                Capacity *= 2;
            }

            _items[Size] = item;
            Size++;
        }

        public void RemoveAt(uint index)
        {
            if (index >= Size)
            {
                throw new IndexOutOfRangeException();
            }

            if (index <= Size - 1)
            {
                Array.Copy(_items, index + 1, _items, index, Size - index - 1);
            }

            Size--;
        }

        public void Clear()
        {
            Size = 0;
            Capacity = 0;
        }


        public override string ToString()
        {
            var sb = new StringBuilder();
            for (var i = 0; i < Size; i++)
            {
                sb.Append($" [{i}] {_items[i]}\n");
            }

            return sb.ToString();
        }
    }
}
