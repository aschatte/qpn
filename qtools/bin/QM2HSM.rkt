#lang racket

(require xml)
(require file/glob)
(require test-engine/racket-tests)
;(require rackunit)

; test-set = #t allows for "internal" testing using the DO_Pwm_Test.qm Hsm model.
(define test-set (make-parameter #f))


; Parse the build tree to get the updated files.
(define default-xml-file "C:\\Projects\\HsmModules\\DO_Pwm\\DO_Pwm_Test_QM\\DO_Pwm_Test_QM.X\\DO_Pwm_Test.qm")
(define xml-model-file (make-parameter default-xml-file))
(unless (test-set)
  (xml-model-file
   (command-line
    #:program "QM2Hsm Converter"
    #:args (filename) ; expect one command-line argument: <filename>
    ; return the argument as a filename to compile
    (first (glob filename)))))

(check-expect (xml-model-file) default-xml-file)

(define find-files-to-process
  (lambda (input)
    (let* ([doc-xml (read-xml input)]
           [doc-element (document-element doc-xml)]
           [doc-element-name (element-name doc-element)]
           [doc-element-attributes (element-attributes doc-element)]
           [doc-element-content (element-content doc-element)]
           )
      (letrec ([filter-content-recursively
                (lambda (file-list content)
                  (if (null? content)
                      (if (null? file-list)
                          '()
                          (list (list 'file file-list)))
                      (let ([first-content (car content)])
                        (cond [(and (element? first-content) (eq? (element-name first-content) 'directory))
                               (cons (cons (element-name first-content)
                                           (cons (first (map (lambda (attr) (attribute-value attr)) (element-attributes first-content)))
                                                 (list (filter-content-recursively '() (element-content first-content)))))
                                     (filter-content-recursively '() (cdr content)))]
                              [(and (element? first-content) (eq? (element-name first-content) 'file))
                               (filter-content-recursively
                                (append file-list (map (lambda (attr) (attribute-value attr)) (element-attributes first-content)))
                                (cdr content))
                               ]
                              [else (filter-content-recursively file-list (cdr content))]
                              ))))])
        (let ([test-results (first (filter-content-recursively '() doc-element-content))])
          test-results)))))
(when (test-set)
  ;  (with-output-to-file (string->path "test.rkt") #:exists 'replace
  ;    (lambda ()
  (pretty-print
   (call-with-input-file* (string->path (xml-model-file)) find-files-to-process)))
;))

(check-expect (call-with-input-file* (string->path (xml-model-file)) find-files-to-process)
              '(directory "."
                          ((directory "DO_Pwm_Test" ((file ("DO_Pwm_Test_QM.c" "DO_Pwm_Test_QM.h"))))
                           (directory "..\\.." ((file ("DO_Pwm_QM.c" "DO_Pwm_QM.h"))))
                           (file ("main.c")))))

; Create a backup copy, and run the set of edits to convert the QM model-nano code to the Hsm code.
(define (QM2Hsm files-to-compile)
  (for ([file files-to-compile])
    (printf "\tProcessing model file path: ~s\n" file)
    (copy-file file (let ([backup-file (when (path? file) (path->string file))]) (string-append backup-file ".bak")) #t)
    (call-with-input-file* file
      (lambda (input)
        (let ([i-str (port->string input)])
          (with-output-to-file file
            #:exists 'replace
            (lambda ()
              (display
               (regexp-replaces i-str
                                '(
                                  ; "DO_Pwm_Test" is the name of the Active Object (AO) being transformed.
                                  ; Fix the state functions' arguments capturing the AO name using #px"\\(([\\w]*) and adding the me pointer extraction in .c files.
                                  ; (DO_Pwm_Test * const me) => (void * me2, HsmEvent const * const e2) {\n    DO_Pwm_Test *me = (DO_Pwm_Test *)me2;
                                  [#px"\\(([\\w]*) \\* const me\\) \\{" "(void * me2) {\n    \\1 *me = (\\1 *)me2;"]  ; for .c file
                                  ; (DO_Pwm_Test * const me); => (void * me2);
                                  [#px"\\(([\\w]*) \\* const me\\);" "(void * me2);"]  ; for .h file
                                  ;                                  ; Fix ctor for both .c & .h filesr
                                  ;                                  ; void DO_Pwm_Test_ctor(void * me2, HsmEvent const * const e2) => void DO_Pwm_Test_ctor(void * me2)"
                                  ;                                  [#px"void ([\\w]*)_ctor\\(void \\* me2\\, HsmEvent const \\* const e2\\)" "void \\1_ctor(void * me2)"]
                                  ;                                  [#px"void ([\\w]*)_ctor\\(void \\* me2\\) \\{\\\n    QEvt \\*e \\= \\(QEvt \\*\\)e2;" "void \\1_ctor(void * me2) {"]
                                  )
                                )))))))))

(define (build-QM-file-list xml-model-file)
  (letrec ([process-QM-XML-struct->file-list
            (lambda (file-path QM-xml-struct)
              (if (null? QM-xml-struct)
                  '()
                  (let ([type (first QM-xml-struct)])
                    (cond [(eq? type 'directory)
                           (let* ([path (second QM-xml-struct)]
                                  [dir-struct (third QM-xml-struct)]
                                  [new-path (if (absolute-path? path) (string->path path) (build-path file-path path))])
                             (map (lambda (dir) (process-QM-XML-struct->file-list new-path dir)) dir-struct))]
                          [(eq? type 'file)
                           (let* ([file-list (second QM-xml-struct)])
                             (map (lambda (file) (build-path file-path file)) file-list))]
                          [else (error "Incorrect structure type.")]))))])
    (printf "\nProcessing QM Model file:~s.\n" (xml-model-file))
    (let* ([QM-xml-struct (call-with-input-file* (xml-model-file) find-files-to-process)]
           [QM-file-list (flatten (process-QM-XML-struct->file-list (path-only (xml-model-file)) QM-xml-struct))])
      QM-file-list)))

(check-expect (QM2Hsm (build-QM-file-list xml-model-file)) (void))

(if (test-set)
    (time (test))
    (time (QM2Hsm (build-QM-file-list xml-model-file))))

