import os
import datetime
import pickle
from google.oauth2.credentials import Credentials
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
from googleapiclient.discovery import build

# 구글 캘린더 API 범위
SCOPES = ['https://www.googleapis.com/auth/calendar']


def authenticate_google_account():
    """구글 계정 인증 및 토큰 생성"""
    creds = None
    # 기존에 저장된 토큰이 있는 경우 로드
    if os.path.exists('token.pickle'):
        with open('token.pickle', 'rb') as token:
            creds = pickle.load(token)
    # 유효한 자격 증명이 없는 경우 새로 로그인
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            # 절대 경로 사용 (raw 문자열)
            flow = InstalledAppFlow.from_client_secrets_file(
                r'C:\Users\ktmth\source\repos\codinghaezo\tm_repository\ConnectedToGoogleCalendar_test_1\projectPro\credentials.json',
                SCOPES
            )
            creds = flow.run_local_server(port=0)
        # 생성된 토큰 저장
        with open('token.pickle', 'wb') as token:
            pickle.dump(creds, token)
    return creds


def get_calendar_service():
    """구글 캘린더 서비스 생성"""
    creds = authenticate_google_account()
    service = build('calendar', 'v3', credentials=creds)
    return service


def read_reminders(file_path):
    """텍스트 파일에서 리마인더 읽기"""
    reminders = []
    if os.path.exists(file_path):
        with open(file_path, 'r') as file:
            lines = file.readlines()
            for line in lines:
                date_str, reminder = line.strip().split(':')
                reminders.append((date_str, reminder))
    return reminders


def add_reminder_to_calendar(service, date_str, reminder):
    """구글 캘린더에 리마인더 추가"""
    date = datetime.datetime.strptime(date_str, '%Y-%m-%d').date()
    start_date_time = datetime.datetime.combine(date, datetime.time(9, 0))
    end_date_time = start_date_time + datetime.timedelta(hours=1)

    event = {
        'summary': reminder,
        'start': {
            'dateTime': start_date_time.isoformat(),
            'timeZone': 'Asia/Seoul',
        },
        'end': {
            'dateTime': end_date_time.isoformat(),
            'timeZone': 'Asia/Seoul',
        },
    }

    service.events().insert(calendarId='primary', body=event).execute()


def get_existing_reminders(service):
    """구글 캘린더에서 기존 리마인더 가져오기"""
    # 현재 시간으로부터 1년 전까지의 일정을 가져오도록 설정
    past_time = (datetime.datetime.utcnow() - datetime.timedelta(days=365)).isoformat() + 'Z'
    events_result = service.events().list(calendarId='primary', timeMin=past_time,
                                          maxResults=1000, singleEvents=True,
                                          orderBy='startTime').execute()
    events = events_result.get('items', [])
    reminders = []
    for event in events:
        start = event['start'].get('dateTime', event['start'].get('date'))
        summary = event.get('summary', '')
        date_str = start.split('T')[0]
        reminders.append((date_str, summary))
    return reminders


def write_reminders(file_path, reminders):
    """리마인더를 텍스트 파일에 쓰기"""
    existing_reminders = read_reminders(file_path)
    all_reminders = {date_str: reminder for date_str, reminder in existing_reminders}

    # 구글 캘린더에서 가져온 리마인더 추가
    for date_str, reminder in reminders:
        all_reminders[date_str] = reminder

    with open(file_path, 'w') as file:
        for date_str, reminder in sorted(all_reminders.items()):
            file.write(f"{date_str}:{reminder}\n")


if __name__ == '__main__':
    service = get_calendar_service()

    # 텍스트 파일에서 리마인더 읽기
    file_path = r"C:\Users\ktmth\source\repos\codinghaezo\tm_repository\Calendar_test_1\Project5\Project5\reminders.txt"
    reminders = read_reminders(file_path)

    # 구글 캘린더에서 기존 리마인더 가져오기
    existing_reminders = get_existing_reminders(service)
    existing_reminder_set = set(existing_reminders)

    # 리마인더를 구글 캘린더에 추가 (중복 방지)
    for date_str, reminder in reminders:
        if (date_str, reminder) not in existing_reminder_set:
            add_reminder_to_calendar(service, date_str, reminder)

    # 리마인더를 텍스트 파일에 쓰기 (기존 내용을 유지하고 추가)
    write_reminders(file_path, existing_reminders)
