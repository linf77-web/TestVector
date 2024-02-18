/*
 *ʹ�÷�����
 ͨ��������䣺CSingleton<����>::Instance()����ȡָ��ĳ��ʵ����ָ�롣
 **/
template<class T>
class CSingleton
{
public:
	static T* Instance()
	{
        if (nullptr == m_pInstance)
		{
			// ���μ��  
            if (nullptr == m_pInstance)
			{
				m_pInstance = new T;
				atexit(Destory);
			}
		}

		return m_pInstance;
	}

protected:
	CSingleton() {} //��ֹʵ��  
	CSingleton(const CSingleton&) {} //��ֹ��������һ��ʵ��  
	CSingleton& operator=(const CSingleton&){} //��ֹ��ֵ����һ��ʵ��  

	virtual ~CSingleton()
	{
	}

	static void Destory()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
            m_pInstance = nullptr;
		}
	}

private:
	static T* m_pInstance;
};

template<class T> T* CSingleton<T>::m_pInstance = nullptr;
