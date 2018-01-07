module StackTypes where


--
-- * Part 1: A Rank-Based Type System for the Stack Language
--
-- ** The abstract syntax
--
type Prog = [Cmd]

data Cmd = Push Int
         | Pop Int
         | Add
         | Mul
         | Dup
         | Inc
         | Swap
         deriving(Eq,Show)

type Stack = [Int]

type Rank = Int

-- NOTES:      (popsnum on stack,pushes on stack)
type CmdRank = (Int,Int)


-- ** The semantic function that yields the semantics of a program
--
prog :: Prog -> Stack -> Maybe Stack
prog []     s = Just s
prog (c:cs) s = cmd c s >>= prog cs


-- ** The semantics function that yields the semantics of a command
--
cmd :: Cmd -> Stack -> Maybe Stack
cmd (Push n) s       = Just (n:s)
cmd (Pop  k) s       = Just (drop k s)
cmd Add      (n:k:s) = Just (n + k:s)
cmd Mul      (n:k:s) = Just (n * k:s)
cmd Dup      (n:s)   = Just (n:n:s)
cmd Inc      (n:s)   = Just (n + 1:s)
cmd Swap     (n:k:s) = Just (k:n:s)
cmd _        _       = Nothing



-- | 1. Define the function rankC that maps each stack operation to its rank
--
rankC :: Cmd -> CmdRank
rankC (Push _) = (0,1)
rankC (Pop x)  = (x,0)
rankC (Add)    = (2,1)
rankC (Mul)    = (2,1)
rankC (Dup)    = (1,2)
rankC (Inc)    = (1,1)
rankC (Swap)   = (2,2)

-- | 2. Define the auxiliary function rankP that computes the rank of programs
--
rankP :: Prog -> Rank -> Maybe Rank
rankP [] x = Just x
rankP cmds x = rank cmds x

rank :: Prog -> Rank -> Maybe Rank
rank [] x = Just x
rank (x:xs) r = let (popsnum,pushsnum) = rankC x in
                    if (popsnum < r) then rank xs (r - popsnum + pushsnum)
                    else Nothing

rankPMapper :: Prog -> [CmdRank]
rankPMapper [] = []
rankPMapper cmds = map rankC cmds

-- | 3. Define the semantic function semStatTC for evaluating stack programs
--
semStatTC :: Prog -> Stack -> Maybe Stack
semStatTC prg stk = if (rankP prg (length stk)) == Nothing then Nothing
                    else prog prg stk


-- | EXTRA CREDIT
--
-- prog' = undefined



-- * Part 2: Runtime Stack
--



-- * Part 3: Static and Dynamic Scope
--