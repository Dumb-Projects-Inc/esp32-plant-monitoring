from math import sqrt
import argparse
import pretty_midi
from librosa import midi_to_hz

parser = argparse.ArgumentParser(
                    prog='Midi To C Array',
                    description='This program converts a midi file to a C array to be played on the ESP32 for DTU course 02112',
                    epilog='Use at you own risk')
parser.add_argument("midi_file", help="The midi file to be converted")


def main(args):
    RESOLUTION = 1
    FS = 1000 / RESOLUTION


    data = []
    midi_path = args.midi_file
    print(f"Reading midi file: {midi_path}")
    midi_data = pretty_midi.PrettyMIDI(midi_path)
    
    roll = midi_data.get_piano_roll(FS)
    pitch_roll = []
    for i in range(len(roll[0])):
        time = []
        for pitch, velocity in enumerate(roll):
            
            if velocity[i] != 0:
                time.append(pitch)
        pitch_roll.append(time)

    hz_roll = [[midi_to_hz(x) for x in time] for time in pitch_roll ]

    finalized_hz = []
    for time, hz_list in enumerate(hz_roll):
        # If no notes are played indicate a pause
        if len(hz_list) == 0:
            finalized_hz.append(0)
            continue
        #Calculate the geometric mean
        gm = 1
        for hz in hz_list:
            gm *= hz
        gm = int(gm ** (1/len(hz_list)))
        
        finalized_hz.append(gm)
    
    last = 0
    for n in finalized_hz:
        if last == n and n != 0:
            data[-2] += int(RESOLUTION)
        elif n == 0:
            if len(data) == 0:
                data.append(100)
                data.append(0)
                data.append(0)
            data[-1] += int(RESOLUTION)
        else:
            last = n
            data.append(n)
            data.append(int(RESOLUTION))
            data.append(20)
    
    #print song
    data.insert(0, int(len(data)))
    print(data)

    

    

if __name__ == "__main__":
    args = parser.parse_args()
    main(args)