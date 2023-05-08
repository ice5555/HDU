package com.count.server.controller;

import org.springframework.http.RequestEntity;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("api/home")
public class home {
    @GetMapping("/test1")
    public ResponseEntity<String> test1(@RequestParam("msg")String msg){
        return ResponseEntity.ok(msg);
    }

    @PostMapping("/test2")
    public ResponseEntity<String> test2(@RequestBody String msg){
        return ResponseEntity.ok(msg);
    }




}
