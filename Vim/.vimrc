" An example for a vimrc file.
"
" Maintainer:   Bram Moolenaar <Bram@vim.org>
" Last change:  2000 Jan 06
"
" To use it, copy it to
"     for Unix and OS/2:  ~/.vimrc
"             for Amiga:  s:.vimrc
"  for MS-DOS and Win32:  $VIM\_vimrc
"               for VMS:  sys$login:.vimrc

" Use Vim settings, rather then Vi settings (much better!).
" This must be first, because it changes other options as a side effect.
set nocompatible

"""""""""""""""""""""my customized vim""""""""""""""""""""
"1-from tips.txt
"map _g :let efsave=&ef<Bar>let &ef=tempname()<Bar>exe ':!grep -n -w "<cword>" *.[cChH] *.cc *.cpp *.txt >'.&ef<CR>:cf<CR>:exe
 ":!rm ".&ef<CR>:let &ef=efsave<Bar>unlet efsave<CR><CR>:cc<CR>
"2-from tips.txt       scroll under insert mode by ^E and ^Y instead of ^X^E and ^X^Y
inoremap <C-E> <C-X><C-E>
inoremap <C-Y> <C-X><C-Y>
"3-from tips.txt     cursor is on the middle of screen forever except at the start and end of file or when long line wrap
"set scrolloff=999
"4-from tips.txt     cursor is between the 
set scrolloff=5
"5-from tips.txt     scroll smooth
"map <C-U> <C-Y><C-Y><C-Y><C-Y><C-Y><C-Y><C-Y><C-Y><C-Y><C-U><C-Y><C-Y><C-Y><C-Y><C-Y><C-Y>
"map <C-D> <C-E><C-E><C-E><C-E><C-E><C-E><C-E><C-E><C-E><C-E><C-E><C-E><C-E><C-E><C-E><C-E>
"6             ignore Upper and Lower case
"set ignorecase
"7       no wrapscan
set nowrapscan
"8       check whether the bracket match or not
set showmatch
"9       auto write current file when go to another file;
set autowrite
"10
"set vimruntime ~
"11
set sm   "match the matching one when type quote
"12  set for HighLight when telnet
if !has("gui_running")
"set background=Dark
set t_Co=8
set t_Sf=[3%p1%dm
set t_Sb=[4%p1%dm
endif
"13 copy text from mth line to local
map ^O "py'm
"14 cut text form mth line to local
map ^K n.
"15  display the current commmand in right-low of screen
set showcmd
"16
"ab te printf("\ntest\n");
"17 set ctags to view multi-tags
map  g
"18 load cscope database
"cscope add $DEV_ROOT_DIR/cscope.out
"19 
set helplang=cn
"20 for taglist
nnoremap <silent> <F8> :TlistOpen<CR>
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
set bs=2                " allow backspacing over everything in insert mode
set ai                  " always set autoindenting on
"set softtabstop=2      " set one tab equal with two spaces
set expandtab           " replace tab with space
set cindent shiftwidth=2 " set indent width as 2 spaces
"set backup             " keep a backup file
set viminfo='20,\"50    " read/write a .viminfo file, don't store more
                        " than 50 lines of registers
set history=50          " keep 50 lines of command line history
set ruler               " show the cursor position at lower-right of screen all the time
set ignorecase          " ignore the difference between lowercase and uppercase
set smartcase           " override the ignorecase setting if the Upper case contained in search pattern
set whichwrap=b,s,<,>,[,]


" Make p in Visual mode replace the selected text with the "" register.

vnoremap p <Esc>:let current_reg = @"<CR>gvdi<C-R>=current_reg<CR><Esc>

" Switch syntax highlighting on, when the terminal has colors
" Also switch on highlighting the last used search pattern.
if &t_Co > 2 || has("gui_running")
 syntax on set hlsearch
endif
set hlsearch
"set nohlsearch

"set Tlist_Ctags_Cmd /usr/bin/ctags

 " In text files, always limit the width of text to 78 characters
 autocmd BufRead *.txt set tw=78

 augroup cprog
  " Remove all cprog autocommands
  au!

  " When starting to edit a file:
  "   For C and C++ files set formatting of comments and set C-indenting on.
  "   For other files switch it off.
  "   Don't change the order, it's important that the line with * comes first.
  autocmd FileType *      set formatoptions=tcql nocindent comments&
  autocmd FileType c,cpp  set formatoptions=croql cindent comments=sr:/*,mb:*,el:*/,://
 augroup END

"colorscheme evening   "set color scheme
colorscheme torte "set color scheme
"source ~/.vim/.icscape.vim

" add by shengmh 2021-03-16
xnoremap * :<C-u>call <SID>VSetSearch()<CR>/<C-R>=@/<CR><CR>
xnoremap # :<C-u>call <SID>VSetSearch()<CR>?<C-R>=@/<CR><CR> 
function! s:VSetSearch() 
  let temp = @s 
  norm! gv"sy 
  let @/ = '\V' . substitute(escape(@s, '/\'), '\n', '\\n', 'g') 
  let @s = temp 
endfunction
