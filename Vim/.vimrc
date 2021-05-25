" no vi
set nocompatible

" syntax
syntax on 

" mode
set showmode

"
set showcmd

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

set backupdir=/home/juzi/.vim/.backup//
set directory=/home/juzi/.vim/.swp//
set undodir=/home/juzi/.vim/.undo//

set noerrorbells

set history=1000

set wildmenu
set wildmode=longest:list,full

" visual search
xnoremap * :<C-u>call <SID>VSetSearch()<CR>/<C-R>=@/<CR><CR>
xnoremap # :<C-u>call <SID>VSetSearch()<CR>?<C-R>=@/<CR><CR>
function! s:VSetSearch()
    let temp = @s
    norm! gv"sy
    let @/ = '\V' . substitute(escape(@s, '/\'), '\n', '\\n', 'g')
    let @s = temp
endfunction

set tabstop=4
set softtabstop=4
set shiftwidth=4
set expandtab

function! MarkdownConf()
    set tabstop=2
    set softtabstop=2
    set shiftwidth=2
    set expandtab
endfunction

if has("autocmd")
    filetype on
    autocmd FileType markdown call MarkdownConf()
endif

