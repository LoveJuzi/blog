set nocompatible              " be iMproved, required
filetype off                  " required

" syntax
syntax on 

" mode
set showmode

" display the current commmand in right-low of screen
set showcmd

" auto write current file when go to another file
set autowrite

" file encoding
set encoding=utf-8

set t_Co=256

filetype indent on 
set autoindent

set textwidth=90

"set wrap

"set linebreak

"set scrolloff=5

set laststatus=2

set showmatch

set hlsearch

set incsearch

set ignorecase

set smartcase

set undofile

set backupdir=~/.vim/.backup/
set directory=~/.vim/.swp/
set undodir=~/.vim/.undo/

set noerrorbells

set history=1000

set wildmenu
set wildmode=longest:list,full
set backspace=indent,eol,start

set ruler

" visual search
xnoremap * :<C-u>call <SID>VSetSearch()<CR>/<C-R>=@/<CR><CR>
xnoremap # :<C-u>call <SID>VSetSearch()<CR>?<C-R>=@/<CR><CR>
function! s:VSetSearch()
  let temp = @s
  norm! gv"sy
  let @/ = '\V' . substitute(escape(@s, '/\'), '\n', '\\n', 'g')
  let @s = temp
endfunction

set tabstop=2
set softtabstop=2
set shiftwidth=2
set expandtab

set guioptions-=m
set guioptions-=T

if has('gui_running')
  colorscheme peachpuff
endif

let g:netrw_banner=0

set noeb

"set guifont=Consolas:h10

winpos 435 175 
"set lines=43 columns=140 

" use your hand
" autocmd BufWritePost *.c,*.cpp,*.h !ctags -R

function! MarkdownConf()
  set tabstop=2
  set softtabstop=2
  set shiftwidth=2
  set expandtab
endfunction

function! CppConf()
  set tabstop=4
  set softtabstop=4
  set shiftwidth=4
  set expandtab
endfunction

function! SchemeConf()
  set tabstop=2
  set softtabstop=2
  set shiftwidth=2
  set expandtab
endfunction

function! MakeConf()
  set noexpandtab
  set tabstop=8
  set shiftwidth=8
  set softtabstop=0
endfunction

function! GLSLConf()
  set tabstop=4
  set softtabstop=4
  set shiftwidth=4
  set expandtab
endfunction

if has("autocmd")
  filetype on
  autocmd BufReadPost *.rkt,*.rktl set filetype=scheme
  autocmd BufNewFile,BufRead *.hlsl,*.fx,*.fxh setfiletype fx
  autocmd BufNewFile,BufRead *.frag,*.vert,*.fp,*.vp,*.glsl setf glsl
  autocmd FileType markdown call MarkdownConf()
  autocmd FileType cpp      call CppConf()
  autocmd FileType hpp      call CppConf()
  autocmd FileType cc       call CppConf()
  autocmd FileType h        call CppConf()
  autocmd FileType c        call CppConf()
  autocmd FileType scheme   call SchemeConf()
  autocmd FileType make     call MakeConf()
  autocmd FileType shader   call ShaderConf()
  autocmd FileType glsl     call GLSLConf()
endif

map <F3> :call CurrentFilePath()<cr>
function CurrentFilePath()
  normal my
  if has('gui_running')
    "execute 'let @+=expand("%:p")'
    execute 'let @+=expand("%:.")'
  else
    execute 'let @1=expand("%:.")'
  endif
  echohl WarningMsg | echo "Successful in get current file path." | echohl None
  normal `y
endfunction

map <C-e> :Ex<cr>
map <C-d> :bf<cr>
