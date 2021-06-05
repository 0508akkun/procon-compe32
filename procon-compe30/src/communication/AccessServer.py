import requests
import json

url = 'http://localhost:8081/matches/'

def getMatches(data):
    headers = {
        'Authorization': 'procon30_example_token',
    }

    response = requests.get(url, headers=headers)
    status = json.loads(response.text)

    if 'status' in status:
        return response.status_code, status['status']
    else:
        return response.status_code, status
    
def getStatus(id):
    headers = {
        'Authorization': 'procon30_example_token',
    }
    response = requests.get(url + str(id), headers=headers)
    status = json.loads(response.text)

    if 'status' in status:
        return response.status_code, status['status']
    else:
        return response.status_code, status

def postActions(matchID, actions):
    headers = {
        'Authorization': 'procon30_example_token',
        'Content-Type': 'application/json',
    }
    response = requests.post(url + str(matchID) + '/action', headers=headers, data=json.dumps(actions))

    status = json.loads(response.text)

    if 'status' in status:
        return response.status_code, status['status']
    else:
        return response.status_code, None

def runPing():
    headers = {
        'Authorization': 'procon30_example_token',
    }

    response = requests.get('http://localhost:8081/ping', headers=headers)
    status = json.loads(response.text)

    return response.status_code, status['status']