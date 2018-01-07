module MiniMiniLogoSem where

import MiniMiniLogo
import Render

--NOTES -----------------------------------------------
-- -- | A program is a sequence of commands.
-- type Prog = [Cmd]

-- -- | The mode of the pen.
-- data Mode = Down | Up
--   deriving (Eq,Show)

-- -- | Abstract syntax of commands.
-- data Cmd = Pen Mode
--          | Move Int Int
--   deriving (Eq,Show)

-- -- | A point is a cartesian pair (x,y).
-- type Point = (Int,Int)

-- -- | A line is defined by its endpoints.
-- type Line = (Point,Point)
-------------------------------------------------------



--
-- * Semantics of MiniMiniLogo
--

-- NOTE:
--   * MiniMiniLogo.hs defines the abstract syntax of MiniMiniLogo and some
--     functions for generating MiniMiniLogo programs. It contains the type
--     definitions for Mode, Cmd, and Prog.
--   * Render.hs contains code for rendering the output of a MiniMiniLogo
--     program in HTML5. It contains the types definitions for Point and Line.

-- | A type to represent the current state of the pen.
type State = (Mode,Point)

-- | The initial state of the pen.
start :: State
start = (Up,(0,0))

-- | A function that renders the image to HTML. Only works after you have
--   implemented `prog`. Applying `draw` to a MiniMiniLogo program will
--   produce an HTML file named MiniMiniLogo.html, which you can load in
--   your browswer to view the rendered image.
draw :: Prog -> IO ()
draw p = let (_,ls) = prog p start in toHTML ls


-- Semantic domains:
--   * Cmd:  State -> (State, Maybe Line)
--   * Prog: State -> (State, [Line])


-- | Semantic function for Cmd.
--
--   >>> cmd (Pen Down) (Up,(2,3))
--   ((Down,(2,3)),Nothing)
--
--   >>> cmd (Pen Up) (Down,(2,3))
--   ((Up,(2,3)),Nothing)
--
--   >>> cmd (Move 4 5) (Up,(2,3))
--   ((Up,(4,5)),Nothing)
--
--   >>> cmd (Move 4 5) (Down,(2,3))
--   ((Down,(4,5)),Just ((2,3),(4,5)))
--
cmd :: Cmd -> State -> (State, Maybe Line)
cmd (Pen x) (_,pt)      = (  (x,pt) , Nothing) 
cmd (Move m1 m2) (Down,pt) = (  (Down, (m1,m2)) , Just((pt),(m1,m2)) )
cmd (Move m1 m2) (Up,pt) = (  (Up, (m1,m2)) , Nothing)


-- | Semantic function for Prog.
--
--   >>> prog (nix 10 10 5 7) start
--   ((Down,(15,10)),[((10,10),(15,17)),((10,17),(15,10))])
--
--   >>> prog (steps 2 0 0) start
--   ((Down,(2,2)),[((0,0),(0,1)),((0,1),(1,1)),((1,1),(1,2)),((1,2),(2,2))])
prog :: Prog -> State -> (State, [Line])
prog prgs st = (progHelper prgs (st,[]) )

progHelper :: Prog -> (State, [Line]) -> (State, [Line])
progHelper [] x = x
progHelper (prg:prgs) (st,ls) = 
                    let (nexst,nexln) = cmd prg st in
                    case nexln of 
                        Just nexln -> progHelper prgs (nexst,ls ++ [nexln]) 
                        Nothing -> progHelper prgs (nexst,ls)

-- | Makes life ez
-- How to run 101
-- ezRander (prog (nix 10 10 5 7) start)
ezRender :: (State,[Line]) -> IO ()
ezRender (_,xs) = toHTML xs

--
-- * Extra credit
--

-- | This should be a MiniMiniLogo program that draws an amazing picture.
--   Add as many helper functions as you want.
-- ezRender(prog amazing start)
amazing :: Prog
amazing = [Pen Up, Move 5 5,Pen Down, Move 20 5, Move 20 10, Move 5 10, Move 5 5, Pen Up] ++ (steps 5 20 10)
          ++ [Pen Up, Move 5 10, Pen Down, Move 3 13, Pen Up] ++ (bigbox 0 13) 
          ++ [Pen Up, Move 0 17, Pen Down, Move 1 19,Move 2 17, Move 3 19, Move 4 17, Pen Up]
          ++ [Pen Up, Move 7 5, Pen Down, Move 7 0, Pen Up]
          ++ [Pen Up, Move 17 5, Pen Down, Move 17 0, Pen Up]

bigbox :: Int -> Int -> Prog
bigbox x y = [Pen Up, Move x y, Pen Down,
           Move (x+4) y, Move (x+4) (y+4), Move x (y+4), Move x y]

