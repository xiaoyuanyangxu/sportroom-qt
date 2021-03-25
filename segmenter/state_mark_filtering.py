import numpy as np
import cv2
import math
import os
import sys
import time

def detect_state_mark_width(frame):
    height, width, channels = frame.shape
    state = 0
    num_black_pixel = 0
    num_white_pixel = 0
    for x in range(0, int(width/2)):
        if state == 0:
            # let find a white pixel
            if frame[ 0, width-x-1,0] > 50:
                state = 1
                num_white_pixel += 1
            else:
                num_black_pixel += 1
        else:
            # let confirm that there are enough white pixel
            if frame[ 0, width-x-1,0] > 10:
                num_white_pixel += 1
                if num_white_pixel > num_black_pixel:
                    break
            else:
                break
    if num_black_pixel > 5 and num_white_pixel > num_black_pixel:
        return num_black_pixel
    else:
        return -1

def ms_to_time(ms):
    s = int(ms/1000)
    ms = int(ms%1000)
    min = int(s/60)
    s = int(s%60)
    h = int(min/60)
    min = int(min%60)

    return "%02i:%02i:%02i.%i"%(h,min,s,ms)

def main():
    if len(sys.argv) < 5:
        print("USAGE: %s VIDEO_FILE TARGET_STATE SECONDS_TO_SKIP MAX_SEGMENT_LENGTH"%(sys.argv[0]))
        print("Example: python3.6 %s test_state_long_long.mp4 1 600 20"%(sys.argv[0]))
        sys.exit(-1)
    
    video_file = sys.argv[1]
    target_state = int(sys.argv[2])
    skip = int(sys.argv[3])
    max_length = int(sys.argv[4])

    now = int(time.time())


    cutter_file_name = "video_cutter_%i.bat"%(now)
    seg_list_file_name = "segment_list_%i.list"%(now)
    cutter_file = open(cutter_file_name, "w")
    seg_list_file = open(seg_list_file_name, "w")
    
    print("Parse %s with target state=%i, skipping %i seconds"%(video_file, target_state, skip))
    
    cutter_file.write("REM Let start")

    cap = cv2.VideoCapture(video_file)
    fps = cap.get(cv2.CAP_PROP_FPS)

    num_frame = 0
    timestamp = 0
    while timestamp < skip:
        ret,frame = cap.read()
        num_frame += 1

        if num_frame % 30 == 0:
            timestamp = cap.get(cv2.CAP_PROP_POS_MSEC) / 1000
        
            if  (num_frame % (30 * 100)) == 0:
                print ("Skiping %i frames %i seconds"%(num_frame, timestamp))
            

    ret,frame = cap.read()
    #Get frame height and width to access pixels
    height, width, channels = frame.shape
    print("Height: %i Width:%i Channels:%i"%(height, width, channels))
    cv2.imwrite("frame.jpg", frame)
    mark_width = detect_state_mark_width(frame)
    if mark_width<0:
        print("Mark is not clean enough. Check frame.jpg")
        return

    print("Mark width: %i pixels"%(mark_width))
    prefix = 0 #5 * 60 +10 

    frame_number = 0
    previous_state = 0
    previous_time = 0
    segment = 0

    while(1):
        ret,frame = cap.read()
        timestamp = cap.get(cv2.CAP_PROP_POS_MSEC)
        frame_number += 1
        if timestamp / 1000 < prefix:
            continue

        if ret == False:
            break
        
        state = 0
        x = 0
        row = []
        mask = 1
        for m in range(0, 6):
            r = 0
            for z in range(0, mark_width):
                if frame[ 0, width-x-1,0] > 50:
                    r += 1
                x += 1
            if r > mark_width*0.5:
                state = state | mask
                row.append("1")
            else:
                row.append("0")
            mask = mask << 1
        state = state >> 2   
        if state != previous_state:
            if previous_state == target_state:
                segment += 1
                start_time = ms_to_time(previous_time)
                length = ms_to_time(timestamp - previous_time)
                l_sec = int((timestamp - previous_time) / 1000.0) 
                output = video_file + ".%i.%i.mkv"%(segment, l_sec)

                
                # Check follow to better cut
                # https://superuser.com/questions/138331/using-ffmpeg-to-cut-up-video
                # "c:\Program Files\ffmpeg\bin\ffmpeg.exe" -ss 00:15:25.233 -i c:\Users\user\Videos\cns_vs_badalona.mkv -c:a aac -ac 2 -ar 48000 -b:a 128k   -c:v h264_nvenc -t 00:00:03.666 c:\Users\user\Videos\cns_vs_badalona.mkv.1.mkv
                cmd = 'ffmpeg.exe -ss %s -i %s -c:a aac -ac 2 -ar 48000 -b:a 128k -c:v h264_nvenc -t %s %s'%(start_time, video_file, length, output)
                print(cmd)
                cutter_file.write("%s\n"%(cmd))
                seg_list_file.write("file '%s'\n"%(output))

                #os.system(cmd)
            print("State Change Detected: %s: %i: %i: %s"%(ms_to_time(timestamp), frame_number, state, "".join(row[2:])))
            previous_state = state
            previous_time = timestamp
    cap.release()


    print("NEXT: run %s"%(cutter_file_name))
    
    ffmpeg_cmd = "ffmpeg.exe -f concat -safe 0 -i %s -c copy ouput_%i.mp4"%(seg_list_file_name, now)
    #ffmpeg.exe -f concat -safe 0 -i .\all_segments.txt -c copy salida.mp4
    print("NEXT: run ./%s"%(ffmpeg_cmd))

    cutter_file.write("REM %s\n"%(ffmpeg_cmd))
    cutter_file.close()
    seg_list_file.close()

if __name__ == "__main__":
    main()