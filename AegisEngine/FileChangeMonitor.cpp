#ifdef _DEBUG
#include	"Texture_Manager.h"

#include "common.h"

FILE_CHANGE_MONITOR::FILE_CHANGE_MONITOR(const aegis::string& directoryName) /*: m_directoryName(directoryName)*/
{
	m_directoryHandle = nullptr;
	m_eventHandle = nullptr;

#ifdef UNICODE
	m_directoryName = stringTowstring(directoryName);
#else
	m_directoryName = directoryName;
#endif // !UNICODE
}

FILE_CHANGE_MONITOR::~FILE_CHANGE_MONITOR()
{
	m_fileActions.clear();
	m_buf.clear();

	CancelIo(m_directoryHandle);
	WaitForSingleObject(m_eventHandle, INFINITE);

	CloseHandle(m_eventHandle);
	CloseHandle(m_directoryHandle);
}

bool FILE_CHANGE_MONITOR::Init()
{
	m_directoryHandle = CreateFile(m_directoryName.c_str(), FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr,
		OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, nullptr);

	if (m_directoryHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	m_buf.reserve(m_bufferSize);
	m_buf.resize(m_bufferSize);

	m_eventHandle = CreateEvent(nullptr, TRUE, FALSE, nullptr);

	beginReadChanges();

	return true;
}

bool FILE_CHANGE_MONITOR::beginReadChanges()
{
	ResetEvent(m_eventHandle);

	Olp = { 0 };
	Olp.hEvent = m_eventHandle;

	const DWORD filter = FILE_NOTIFY_CHANGE_LAST_WRITE;

	if (!ReadDirectoryChangesW(m_directoryHandle, m_buf.data(), m_bufferSize, FALSE, filter, nullptr, &Olp, nullptr))
	{
		return false;
	}

	return true;
}

bool FILE_CHANGE_MONITOR::Get_FileStack_Empty()
{
	return m_fileActions.empty();
}

aegis::wstring FILE_CHANGE_MONITOR::Pop_FileStack()
{
	aegis::wstring file_name = *(m_fileActions.begin());
	m_fileActions.erase(m_fileActions.begin());
	return file_name;
}

void FILE_CHANGE_MONITOR::readChanges()
{
	auto waitResult = WaitForSingleObject(m_eventHandle, 0);
	if (WAIT_TIMEOUT == waitResult)
	{
		return;
	}

	DWORD retsize = 0;
	if (!GetOverlappedResult(m_directoryHandle, &Olp, &retsize, FALSE))
	{
		beginReadChanges();
		return;
	}

	if (0 == retsize)
	{
		beginReadChanges();
		return;
	}
	auto pData = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(&m_buf[0]);

	while (true)
	{
		if (pData->NextEntryOffset == 0)
		{
			beginReadChanges();
			return;
		}

		aegis::wstring m_fileName = aegis::wstring(pData->FileName).substr(0, pData->FileNameLength / sizeof(wchar_t));

		m_fileActions.insert(m_fileName);

		pData = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(reinterpret_cast<unsigned char*>(pData) + pData->NextEntryOffset);
	}
}

aegis::unordered_set<aegis::wstring>* FILE_CHANGE_MONITOR::Get()
{
	return &m_fileActions;
}
#endif // _DEBUG