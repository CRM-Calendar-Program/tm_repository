import os
import pickle
import chardet
from google.oauth2.credentials import Credentials
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
from google.cloud import language_v1


# 클라이언트 시크릿 파일 경로 (key_path로 지정)
key_path = r'C:\Users\ktmth\Desktop\client_secret_.apps.googleusercontent.com.json'

# Google Cloud Natural Language API의 스코프
SCOPES = ['https://www.googleapis.com/auth/cloud-language']


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
            flow = InstalledAppFlow.from_client_secrets_file(
                key_path,  # key_path 사용
                SCOPES
            )
            creds = flow.run_local_server(port=0)
        # 생성된 토큰 저장
        with open('token.pickle', 'wb') as token:
            pickle.dump(creds, token)
    return creds


def read_text_file(file_path):
    """텍스트 파일에서 내용 읽기"""
    with open(file_path, 'rb') as file:
        raw_data = file.read()
        result = chardet.detect(raw_data)
        encoding = result['encoding']

    with open(file_path, 'r', encoding=encoding) as file:
        return file.read()


def summarize_text(text, key_path):
    """텍스트 요약 (Google Cloud Natural Language API 사용)"""
    # Google Cloud 서비스 계정 인증
    os.environ['GOOGLE_APPLICATION_CREDENTIALS'] = key_path
    client = language_v1.LanguageServiceClient()

    document = language_v1.Document(
        content=text,
        type_=language_v1.Document.Type.PLAIN_TEXT
    )

    # 텍스트를 요약하는 대신 문장을 분석하여 요약하기
    response = client.annotate_text(
        request={'document': document,
                 'features': {'extract_syntax': True},
                 'encoding_type': language_v1.EncodingType.UTF8}
    )

    # 분석된 문장에서 요약 생성
    sentences = [sentence.text.content for sentence in response.sentences]
    summary = ' '.join(sentences[:5])  # 예시: 첫 5개의 문장을 요약으로 사용

    return summary


if __name__ == '__main__':
    file_path = r"C:\Users\ktmth\source\repos\codinghaezo\tm_repository\Calendar_test_1\Project5\Project5\reminders.txt"
    text = read_text_file(file_path)
    summary = summarize_text(text, key_path)

    print("Summary:")
    print(summary)
