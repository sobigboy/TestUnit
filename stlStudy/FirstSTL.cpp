#include <stdio.h>

template<class T>
class MyArray
{
private:
	int m_nTotalSize;
	int m_nValidSize;
	T* m_pData;

public:
	MyArray(int nSize = 2)
	{
		m_pData = new T[nSize];
		m_nTotalSize = nSize;
		m_nValidSize = 0;
	}
	virtual ~MyArray()
	{
		if(m_pData)
		{
			delete []m_pData;
			m_pData = NULL;
		}
	}

	void Add(T value)
	{
		if(m_nValidSize >= m_nTotalSize)
		{
			T* tempData = new T[m_nTotalSize];

			for(int i = 0; i < m_nTotalSize; i ++)
			{
				tempData[i] = m_pData[i];
			}
			delete []m_pData;

			m_nTotalSize *= 2;
			m_pData = new T[m_nTotalSize];

			for(int i = 0; i < m_nTotalSize; i ++)
			{
				m_pData[i] = tempData[i];
			}
			delete []tempData;

		}
		m_pData[m_nValidSize] = value;
		m_nValidSize ++;
	}

	int GetSize()
	{
		return m_nValidSize;
	}

	T Get(int nPos)
	{
		return m_pData[nPos];
	}
	
};


int main()
{
	MyArray<int>obj;
	obj.Add(1);
	obj.Add(2);
	obj.Add(3);
	obj.Add(4);

	for(int i = 0; i < obj.GetSize(); i++)
	{
		printf("%d: %d\n", i, obj.Get(i));
	}

	return 0;
}