# procon32\_example
## Installation
```
python3 -m venv .venv
source .venv/bin/activate
pip install -U pip
pip install -r requirements.txt
```

## How to use
```
# download problem.ppm
python procon32.py download --token=your_token

# submit solution
python procon32.py submit --token=your_token -f solution.txt
```
