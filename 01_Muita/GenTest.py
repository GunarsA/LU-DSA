# Typehints within class definition can use that class
from __future__ import  annotations

from enum import Enum
import random

# File
OUT_FILE = "customs.in"

# Tokens - P or N -> Do not touch
class Tokens(Enum):
    NONE = 'idk, some random none value'
    P_TYPE = 'P'
    N_TYPE = 'N'

    def __init__(self, token : str):
        self.token = token

    @staticmethod
    def pick_one() -> Tokens:
        if bool(random.randint(0, 1)):
            return Tokens.P_TYPE
        else:
            return Tokens.N_TYPE
    pass

# Ranges -> Do not touch
class Ranges(Enum):
    MUITNIEKU_COUNT  = (1, 99)
    MUITNIEKU_W_TIME = (1, 100000)
    OUTLIERS         = (0, -1) # Second val should always be modified
    IN_PEOPLE_COUNT  = (0, 4000000)

    def __init__(self, min_v : int, max_v :int):
        self.min_v = min_v
        self.max_v = max_v

    def get_random(self) -> int:
        assert (self.min_v == -1 or self.max_v == -1)
        return random.randint(self.min_v, self.max_v)

    # value of -1 translates to ingore this limit / use the default one
    def get_random_wighted(self, min_v : int, max_v : int) -> int:
        if(self.min_v == -1 or self.max_v == -1):
            return self._pick_random_overide(min_v, max_v)
        assert((self.min_v <= min_v <= self.max_v) or min_v == -1)
        assert((self.min_v <= max_v <= self.max_v) or max_v == -1)
        if(min_v == -1 and max_v == -1):
            return self.get_random()
        if min_v == -1:
            return random.randint(self.min_v, max_v)
        if max_v == -1:
            return random.randint(min_v, self.max_v)
        return random.randint(min_v, max_v)

    def _pick_random_overide(self, min_v : int, max_v : int) -> int:
        if (self.min_v == -1 and self.max_v == -1):
            return random.randint(min_v, max_v)
        if( self.min_v == -1):
            assert (min_v <= self.max_v)
            return random.randint(min_v, self.max_v)
        if (self.max_v == -1):
            assert (self.min_v <= max_v)
            return random.randint(self.min_v, max_v)
        pass

def main():
    ofile = open(OUT_FILE, 'wt')
    # Firts Line
    P_COUNT = Ranges.MUITNIEKU_COUNT.get_random_wighted(50, 60)
    N_COUNT = Ranges.MUITNIEKU_COUNT.get_random_wighted(50, 60)
    P_DEF_PROC_TIME = Ranges.MUITNIEKU_W_TIME.get_random_wighted(180, 200)
    N_DEF_PROC_TIME = Ranges.MUITNIEKU_W_TIME.get_random_wighted(180, 200)
    ofile.write(f"{P_COUNT} {N_COUNT} {P_DEF_PROC_TIME} {N_DEF_PROC_TIME}\n")

    # Muitnieku Procesing Times
    # Do not make the work times too small or your queues will have no work!
    MAX_OUTL = P_COUNT + N_COUNT
    OUTLIER_COUNT = Ranges.OUTLIERS.get_random_wighted(-1, int(MAX_OUTL / 3))
    OUTLIER_COUNT = 4
    for i in range(OUTLIER_COUNT):
        token = Tokens.pick_one()
        w_time = Ranges.MUITNIEKU_W_TIME.get_random_wighted(180, 230)
        if(token == Tokens.P_TYPE):
            muitn = random.randint(1, P_COUNT)
        elif(token == Tokens.N_TYPE):
            muitn = random.randint(1, N_COUNT)
        else: assert (False)
        # My implimentantion can handdle dublicte enteries here, so i dont check for it.
        ofile.write(f"T {token.token} {muitn} {w_time} \n")

    # And now the ppl
    # PPL_COUNT = Ranges.IN_PEOPLE_COUNT.get_random_wighted(2000000, 2000000)
    PPL_COUNT = 2_000_000
    _MAX_ARIVAL_TIME = 4_000_000
    INIT_ARIVAL_TIME = random.randint(100, 200)
    cur_time = INIT_ARIVAL_TIME
    real_ppl_count = -1
    for i in range(PPL_COUNT):
        token = Tokens.pick_one()
        ofile.write(f"{token.token} {cur_time}\n")
        # Random delta time
        cur_time += random.randint(1, 2)
        if(cur_time >= _MAX_ARIVAL_TIME):
            real_ppl_count = i
            break
        pass

    # EOF
    ofile.write('X')

    # Stats
    print(f"=== Stats ===")
    print(f"Pc: {P_COUNT} Pt: {P_DEF_PROC_TIME}")
    print(f"Nc: {N_COUNT} Nc: {N_DEF_PROC_TIME}")
    print(f"Outlier count: {OUTLIER_COUNT}")
    print(f"Base arival time: {INIT_ARIVAL_TIME:,}")
    print(f"Final arival time: {cur_time:,}")
    print(f"PPL Count Target: {PPL_COUNT:,}")
    print(f"PPL Count Real: {real_ppl_count:,}")

    ofile.close()

if __name__ == '__main__':
    main()








