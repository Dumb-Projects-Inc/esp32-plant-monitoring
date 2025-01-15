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
            end_time = None
            for note in instrument.notes:
                if end_time is not None:
                    data.append(int((end_time - note.start) * 1000))                    

                data.append(int(midi_to_hz(note.pitch)))
                data.append(int((note.end - note.start) * 1000))
                
                end_time = note.end

            data.append(0)
            break

    print(data[:3*100])

    

if __name__ == "__main__":
    args = parser.parse_args()
    main(args)