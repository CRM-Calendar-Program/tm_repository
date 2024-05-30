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
    """ 구글 계정 인증 및 토큰 생성 """
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
    """ 구글 캘린더 서비스 생성 """
    creds = authenticate_google_account()
    service = build('calendar', 'v3', credentials=creds)
    return service


def list_upcoming_events(service, max_results=10):
    """ 다가오는 이벤트 목록 출력 """
    now = datetime.datetime.utcnow().isoformat() + 'Z'
    events_result = service.events().list(calendarId='primary', timeMin=now,
                                          maxResults=max_results, singleEvents=True,
                                          orderBy='startTime').execute()
    events = events_result.get('items', [])
    if not events:
        print('다가오는 이벤트가 없습니다.')
    for event in events:
        start = event['start'].get('dateTime', event['start'].get('date'))
        print(f"{start} - {event['summary']}")


def add_event(service, summary, location, description, start_time, end_time):
    """ 새로운 이벤트 추가 """
    event = {
        'summary': summary,
        'location': location,
        'description': description,
        'start': {
            'dateTime': start_time,
            'timeZone': 'Asia/Seoul',
        },
        'end': {
            'dateTime': end_time,
            'timeZone': 'Asia/Seoul',
        },
    }
    event = service.events().insert(calendarId='primary', body=event).execute()
    print(f"이벤트가 생성되었습니다: {event.get('htmlLink')}")


if __name__ == '__main__':
    service = get_calendar_service()
    # 다가오는 이벤트 목록 출력
    list_upcoming_events(service)

    # 새로운 이벤트 추가 예제
    add_event(service,
              summary='회의',
              location='서울',
              description='프로젝트 회의',
              start_time='2024-06-01T10:00:00',
              end_time='2024-06-01T11:00:00')
