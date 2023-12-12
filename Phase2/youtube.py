from googleapiclient.discovery import build
import random

def get_comment_api():

    with open('.env', 'r') as f:
        api_key = f.read().split("=")[1].strip()

    youtube = build('youtube', 'v3', developerKey=api_key)
    video_id = "dQw4w9WgXcQ"
    data = []
    for i in range(random.randint(0, 10)):
        request = youtube.commentThreads().list(part="snippet,replies", videoId=video_id, textFormat="plainText", maxResults=100)
        response = request.execute()

        data = []
        for item in response['items']:
            
            comment = item['snippet']['topLevelComment']['snippet']['textDisplay']
            
            user_name = item['snippet']['topLevelComment']['snippet']['authorDisplayName'].strip()[1:]
            data.append({"user_name": user_name, "comment": comment})

        request = youtube.commentThreads().list_next(request, response)

    return data 
    