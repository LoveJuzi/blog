set nocompatible              " be iMproved, required
filetype off                  " required

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()

" let Vundle manage Vundle, required
Plugin 'VundleVim/Vundle.vim'
"Plugin 'Valloric/YouCompleteMe'

" All of your Plugins must be added before the following line
call vundle#end()            " required
"
filetype plugin indent on    " required
"
" Brief help
" :PluginList       - lists configured plugins
" :PluginInstall    - installs plugins; append `!` to update or just :PluginUpdate
" :PluginSearch foo - searches for foo; append `!` to refresh local cache
" :PluginClean      - confirms removal of unused plugins; append `!` to auto-approve removal
"
" see :h vundle for more details or wiki for FAQ
" Put your non-Plugin stuff after this line

let g:ycm_python_binary_path = '/usr/bin/python3'
let g:ycm_global_ycm_extra_conf='~/.ycm_extra_conf.py'  "设置全局配置文件的路径
let g:ycm_seed_identifiers_with_syntax=1    " 语法关键字补全
let g:ycm_confirm_extra_conf=1  " 打开vim时不再询问是否加载ycm_extra_conf.py配置
let g:ycm_add_preview_to_completeopt = 0
let g:ycm_show_diagnostics_ui = 0
let g:ycm_server_log_level = 'info'
let g:ycm_min_num_identifier_candidate_chars = 2
let g:ycm_collect_identifiers_from_comments_and_strings = 1
let g:ycm_complete_in_strings=1
let g:ycm_key_invoke_completion = '<c-z>'
set completeopt=menu,menuone
let g:ycm_semantic_triggers =  {
            \ 'c,cpp,python,java,go,erlang,perl': ['re!\w{2}'],
            \ 'cs,lua,javascript': ['re!\w{2}'],
            \ }
let g:ycm_filetype_whitelist = { 
            \ "c":1,
            \ "cpp":1, 
            \ "objc":1,
            \ "sh":1,
            \ "zsh":1,
            \ "zimbu":1,
            \ }
" 跳转快捷键
"nnoremap <C-k> :YcmCompleter GoToDeclaration<CR>|
"nnoremap <C-h> :YcmCompleter GoToDefinition<CR>|
"nnoremap <C-j> :YcmCompleter GoToDefinitionElseDeclaration<CR>|

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

set backupdir=/home/juzi/.vim/.backup/
set directory=/home/juzi/.vim/.swp/
set undodir=/home/juzi/.vim/.undo/

set noerrorbells

set history=1000

set wildmenu
set wildmode=longest:list,full
set backspace=indent,eol,start

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

colorscheme peachpuff

let g:netrw_banner=0

set noeb

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

if has("autocmd")
  filetype on
  autocmd BufReadPost *.rkt,*.rktl set filetype=scheme
  autocmd FileType markdown call MarkdownConf()
  autocmd FileType cpp      call CppConf()
  autocmd FileType hpp      call CppConf()
  autocmd FileType cc       call CppConf()
  autocmd FileType h        call CppConf()
  autocmd FileType c        call CppConf()
  autocmd FileType scheme   call SchemeConf()
  autocmd FileType make     call MakeConf()
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
