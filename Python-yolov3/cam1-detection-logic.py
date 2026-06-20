# cam 1 python script 
import cv2
import numpy as np 
import urllib.request 
import time

# --- 1. SETTINGS ---
url_phone = 'http://192.0.0.4:8080/video'
url_esp32_signal = 'http://172.18.170.100/signal?state='
whT = 416
confThreshold = 0.7
nmsThreshold = 0.3 
classesfile = 'coco.names
classNames = []
with open(classesfile, 'rt') as f:
classNames = f.read().rstrip('\n').split('\n')
net = cv2.dnn.readNetFromDarknet('yolov3.cfg', 'yolov3.weights') 
net.setPreferableBackend(cv2.dnn.DNN_BACKEND_OPENCV) 
net.setPreferableTarget(cv2.dnn.DNN_TARGET_CPU)
# --- NEW: State Tracker to prevent ESP32 crashing ---
last_sent_val = "0"

# --- 2. THE DETECTION FUNCTION ---
ALERT_LIST = ['bird', 'cat', 'dog', 'horse', 'bear']
def findObject(outputs, im):
global last_sent_val # Access the tracker 
hT, wT, cT = im.shape
alert_detected = False 
person_detected = False
bbox, classIds, confs = [], [], []
for output in outputs: 
for det in output:
scores = det[5:]
classId = np.argmax(scores) 
confidence = scores[classId]
if confidence > confThreshold: 
label = classNames[classId]
if label == 'person' or label in ALERT_LIST: 
w, h = int(det[2]*wT), int(det[3]*hT)
x, y = int((det[0]*wT)-w/2), int((det[1]*hT)-h/2) 
bbox.append([x, y, w, h]) 
classIds.append(classId) 
confs.append(float(confidence))
indices = cv2.dnn.NMSBoxes(bbox, confs, confThreshold, nmsThreshold) 
for i in indices:
idx = i if isinstance(i, (int, np.int32, np.int64)) else i[0] 
label = classNames[classIds[idx]]
if label in ALERT_LIST: 
alert_detected = True
if label == 'person': 
person_detected = True
x, y, w, h = bbox[idx]
color = (0, 0, 255) if label in ALERT_LIST else (0, 255, 0) 
cv2.rectangle(im, (x, y), (x + w, y + h), color, 2)
cv2.putText(im, label.upper(), (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.6, color, 2)

# --- ORIGINAL LOGIC KEPT EXACTLY THE SAME ---
# Person seen = safe = send 0 
# Animal seen = alert = send 1 
# Nothing seen = safe = send 0
if alert_detected and not person_detected: 
val = "1"
else:
val = "0"

# --- IMPROVED SENDING LOGIC ---
# Only send if the state changed to avoid overwhelming the ESP32 
if val != last_sent_val:
try:
print(f"Status Change Detected: Sending {val} to ESP32...") 
urllib.request.urlopen(url_esp32_signal + val, timeout=0.2) 
last_sent_val = val # Update the tracker
except Exception as e: 
print(f"Connection Error: {e}")

# --- 3. RUNNING THE SYSTEM ---
cap = cv2.VideoCapture(url_phone) 
print("System Live. Press 'q' to stop.") 
while True:
  for _ in range(3): 
cap.grab()
success, img = cap.read() 
if not success:
print("Waiting for Mobile Video...") 
continue
blob = cv2.dnn.blobFromImage(img, 1/255, (whT, whT), [0, 0, 0], 1, crop=False) 
net.setInput(blob)
layer_names = net.getLayerNames()
outputNames = [layer_names[i - 1] for i in net.getUnconnectedOutLayers()] 
outputs = net.forward(outputNames)
findObject(outputs, img)
cv2.imshow('Smart Farm Security (Mobile Node)', img) 
if cv2.waitKey(1) & 0xFF == ord('q'):
break
cap.release() 
cv2.destroyAllWindows()
