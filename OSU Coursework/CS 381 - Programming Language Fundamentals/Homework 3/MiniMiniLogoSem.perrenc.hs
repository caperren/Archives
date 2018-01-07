module MiniMiniLogoSem where

import MiniMiniLogo
import Render


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
cmd (Pen pen_state) (_, new_point) =  ((pen_state, new_point), Nothing)
cmd (Move mx my) (Up, (start_point)) = ((Up, (mx, my)), Nothing)
cmd (Move mx my) (Down, (start_x, start_y)) = ((Down, (mx, my)), Just ((start_x, start_y),(mx, my)))


-- | Semantic function for Prog.
--
--   >>> prog (nix 10 10 5 7) start
--   ((Down,(15,10)),[((10,10),(15,17)),((10,17),(15,10))])
--
--   >>> prog (steps 2 0 0) start
--   ((Down,(2,2)),[((0,0),(0,1)),((0,1),(1,1)),((1,1),(1,2)),((1,2),(2,2))])
prog :: Prog -> State -> (State, [Line])
prog [] prog_state = (prog_state, [])
prog prog_list start_state = progLineListHelper prog_list (start_state, [])

-- Helper function to handle keeping track of the line list as we move through the program elements      
progLineListHelper :: Prog -> (State, [Line]) -> (State, [Line])
progLineListHelper [] state_and_line_list = state_and_line_list
progLineListHelper (progs_head:progs_tail) (initial_state, initial_line_list) = 
    let (new_state, new_line_list) = cmd progs_head initial_state in
    case new_line_list of 
        Just new_line_list -> progLineListHelper progs_tail (new_state, initial_line_list ++ [new_line_list]) 
        Nothing -> progLineListHelper progs_tail (new_state, initial_line_list)    


--
-- * Extra credit
--

-- | This should be a MiniMiniLogo program that draws an amazing picture.
--   Add as many helper functions as you want.
amazing :: Prog
amazing = undefined
