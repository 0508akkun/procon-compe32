import numpy as np
import time
import random

def solver(points, allyTiled, enemyTiled):
    # なんか解く
    actions = {
        "actions": [
            {"agentID": 2, "dx": 1, "dy": 1, "type": "move"},
            {"agentID": 3, "dx": 1, "dy": 1, "type": "move"},
        ]
    }
    time.sleep(random.randint(3, 5))
    return actions
    