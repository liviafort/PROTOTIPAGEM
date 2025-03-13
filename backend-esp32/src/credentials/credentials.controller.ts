import { Controller, Get, Post, Body } from '@nestjs/common';
import { InjectRepository } from '@nestjs/typeorm';
import { Repository } from 'typeorm';
import { Wifi } from './wifi-credentials.entity';

@Controller('wifi')
export class WifiController {
  constructor(
    @InjectRepository(Wifi)
    private readonly wifiRepo: Repository<Wifi>,
  ) {}

  @Get()
  async get() {
    const wifi = await this.wifiRepo.findOne({ where: { id: 1 } });
    return wifi || { ssid: 'DefaultSSID', password: 'defaultPassword' };
  }

  @Post()
  async post(@Body() body: { ssid: string; password: string }) {
    await this.wifiRepo.upsert({ id: 1, ...body }, ['id']);
    return { success: true };
  }
}