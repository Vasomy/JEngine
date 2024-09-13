
#include<vector>
#ifndef JArray
#define JArray

template<typename T>
class Array
{
public:
	Array() {
		_array = std::vector<T>();
	}
	Array(size_t size) {
		_array = std::vector<T>(size);
	}
	void push_back(const T& val) {
		_array.push_back(val);
	}
	T& front() {
		return _array.front();
	}
	size_t size()const
	{
		return _array.size();
	}
	void reset()
	{
		_array.clear();
		_array.shrink_to_fit();
	}
	T& operator[](int idx)
	{
		return _array[idx];
	}
private:
	std::vector<T>_array;
};
#endif // !JArray
