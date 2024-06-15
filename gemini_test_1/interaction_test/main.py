import google.generativeai as genai
import os
#model = genai.GenerativeModel('gemini-pro')
model = genai.GenerativeModel('gemini-1.5-flash-latest')

def summarize_text(text):
    response = model.generate_content(f"{text}\n\n위 내용을 요약해 줘.")
    return response.text

def read_file(file_path):
    # reminders.txt 파일 읽기 (combine project)
    with open(file_path, 'r', encoding='utf-8') as file:
        return file.read()

def write_file(file_path, text):
    # summary.txt 파일 쓰기 (combine project)
    with open(file_path, 'w', encoding='utf-8') as file:
        file.write(text)


def main():
    input_file_path = r'C:\Users\ktmth\source\repos\codinghaezo\combine_project\combine_project\reminders.txt'  # 입력 파일 경로 설정
    output_file_path = r'C:\Users\ktmth\source\repos\codinghaezo\combine_project\combine_project\summary.txt'  # 출력 파일 경로 설정

    text = read_file(input_file_path)
    summary = summarize_text(text)

    print("요약 결과:")
    print(summary)

    write_file(output_file_path, summary)

if __name__ == "__main__":
    main()
