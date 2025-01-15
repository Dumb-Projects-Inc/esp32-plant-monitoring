import argparse
import pretty_midi
from librosa import midi_to_hz

parser = argparse.ArgumentParser(
                    prog='Midi To C Array',
                    description='This program converts a midi file to a C array to be played on the ESP32 for DTU course 02112',
                    epilog='Use at you own risk')
parser.add_argument("midi_file", help="The midi file to be converted")
parser.add_argument("output_file", help="The output file to be written to")


def main(args):

    data = []
    midi_path = args.midi_file
    out = args.output_file
    print(f"Reading midi file: {midi_path}")
    midi_data = pretty_midi.PrettyMIDI(midi_path)
    for instrument in midi_data.instruments:
        if not instrument.is_drum:
            print(f"Found instrument: {instrument.name}")
            for note in instrument.notes:
                data.append(int(midi_to_hz(note.pitch)))
                data.append(int((note.end - note.start) * 1000))
                data.append(int(100))
            
            #last = 0
            #time = 0
            #for c in instrument.get_piano_roll():
            #    for n in c:
            #        hz = 0
            #        if n > 0:
            #            hz = int(midi_to_hz(n))
            #            #print(f"Note: {n} with Hz: {midi_to_hz(n):.2f}")
            #            
            #        if hz == last:
            #            time += 1
            #        else:
            #            if last == 0 and len(data) > 0:
            #                data.append(time)
            #            else:
            #                data.append(last)
            #                data.append(time)
            #                #data.append(0)
            #            time = 0
            #        last = hz

            break
    print(data[:3*100])

    

if __name__ == "__main__":
    args = parser.parse_args()
    main(args)